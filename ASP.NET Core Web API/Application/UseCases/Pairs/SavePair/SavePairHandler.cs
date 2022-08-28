using System.Threading;
using System.Threading.Tasks;
using Domain.RepositoryInterfaces;
using MediatR;

namespace Application.UseCases.Pairs.SavePair;

public class SavePairHandler : IRequestHandler<SavePairRequest, SavePairResponse>
{
    private readonly IPairRepository _pairRepository;

    public SavePairHandler(IPairRepository pairRepository)
    {
        _pairRepository = pairRepository;
    }
        
    public async Task<SavePairResponse> Handle(SavePairRequest request, CancellationToken cancellationToken)
    {
        request.Validate();

        var isSuccess = await _pairRepository.SaveAsync(request.Pair, cancellationToken);

        return new SavePairResponse(isSuccess);
    }
}