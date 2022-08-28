using Application.UseCases.Pairs.DeletePair;
using Domain.RepositoryInterfaces;

namespace UnitTests.Application.UseCases.Pairs;

[TestFixture]
public class DeletePairTests
{
    private DeletePairHandler _deletePairHandler = null!;
    private readonly Mock<IPairRepository> _fakePairRepository = new();

    [OneTimeSetUp]
    public void OneTimeSetUp()
    {
        _deletePairHandler = new DeletePairHandler(_fakePairRepository.Object);
    }

    [SetUp]
    public void Setup()
    {
        _fakePairRepository.Invocations.Clear();
    }

    [TestFixture]
    public class HandleMethod: DeletePairTests
    {
        [TestCase(true)]
        [TestCase(false)]
        public async Task InputIsValid_ReturnsResponse(bool doesEntityExistInDb)
        {
            // Arrange
            var request = new DeletePairRequest(1);

            _fakePairRepository
                .Setup(x => x.DeleteAsync(request.Id, It.IsAny<CancellationToken>()))
                .ReturnsAsync(doesEntityExistInDb);

            // Act
            var actual = await _deletePairHandler.Handle(request, default);

            // Assert
            Assert.AreEqual(doesEntityExistInDb, actual.IsSuccess);
        }
        
        [TestCase(0)]
        [TestCase(-1)]
        public void InputIsNotValid_ThrowsException(int id)
        {
            // Arrange
            var request = new DeletePairRequest(id); // Id must be positive

            // Act and Assert
            Assert.ThrowsAsync<ArgumentException>(async () => await _deletePairHandler.Handle(request, default));
            _fakePairRepository.VerifyNoOtherCalls();
        }
    }
}