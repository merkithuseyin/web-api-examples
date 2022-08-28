using System.Threading;
using System.Threading.Tasks;
using Domain.RepositoryInterfaces;
using MediatR;

namespace Application.UseCases.Pairs.UpdatePair;

public class UpdatePairHandler : IRequestHandler<UpdatePairRequest, UpdatePairResponse>
{
    private readonly IPairRepository _pairRepository;

    public UpdatePairHandler(IPairRepository pairRepository)
    {
        _pairRepository = pairRepository;
    }
        
    public async Task<UpdatePairResponse> Handle(UpdatePairRequest request, CancellationToken cancellationToken)
    {
        request.Validate();

        var isSuccess = await _pairRepository.UpdateAsync(request.Id, request.Pair, cancellationToken);

        return new UpdatePairResponse(isSuccess);
    }
}